import { Component, OnInit, ChangeDetectorRef, ViewChild } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FormControl } from '@angular/forms';
import { Subscription } from 'rxjs';
import { Router, ActivatedRoute } from '@angular/router';
import { PageEvent } from '@angular/material';
import { MatTableDataSource } from '@angular/material';
import { FhqService } from '../../services/fhq.service';
import { Location } from '@angular/common';

export interface UsefulLinkElement {
  id: number;
  link: string;
  description: string;
  rating: number;
  userFavorites: number;
  userClicks: number;
  userClicksLabel: string;
  favorite: boolean;
  tags: Array<string>;
}

export interface UsefulLinkTag {
  name: string;
  counter: number;
}

@Component({
  selector: 'app-useful-links',
  templateUrl: './useful-links.component.html',
  styleUrls: ['./useful-links.component.css']
})
export class UsefulLinksComponent implements OnInit {
  public usefullLinksData: UsefulLinkElement[] = [];
  subscription: any;
  pageEvent: PageEvent;
  pageIndex: number = 0;
  pageSize: number = 5;
  length: number = 0;
  pageSizeOptions = [5, 10, 25, 50];

  tagList: UsefulLinkTag[] = [];
  filterByTag: string = "";

  errorMessage: string = null;

  searchValue: String = '';
  searchControl = new FormControl('');
  formCtrlSub: Subscription;

  dataSource = new MatTableDataSource<UsefulLinkElement>();
  displayedColumns: string[] = ['usefulLinkData'];

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _router: Router,
    private _fhq: FhqService,
    private _location: Location,
    private _activatedRoute: ActivatedRoute,
  ) { }

  ngOnInit() {
    this._activatedRoute.queryParams.subscribe(params => {
      if (params["search"]) {
        this.searchValue = params["search"];
      }

      if (params["tag"]) {
        this.filterByTag = params["tag"];
      }

      if (params["page_index"]) {
        this.pageIndex = parseInt(params["page_index"],0);
      }

      if (params["page_size"]) {
        this.pageSize = parseInt(params["page_size"],0);
      }
      this.subscription = this._fhq.changedState
        .subscribe(() => this.updatePage());
    });

    this._spinner.hide();
    this.formCtrlSub = this.searchControl.valueChanges
    .debounceTime(1000)
    .subscribe((newValue) => {
      this.searchValue = newValue
      this.updatePage();
    });
  }

  successUsefulLinksList(r: any) {
    this._spinner.hide();

    let newQueryParams = {}
    if (this.searchValue != "") {
      newQueryParams["search"] = this.searchValue;
    }
    if (this.filterByTag != "") {
      newQueryParams["tag"] = this.filterByTag;
    }
    if (this.pageIndex != 0) {
      newQueryParams["page_index"] = this.pageIndex;
    }
    if (this.pageSize != this.pageSizeOptions[0]) {
      newQueryParams["page_size"] = this.pageSize;
    }
    
    const url = this._router.createUrlTree([], {
      relativeTo: this._activatedRoute, 
      queryParams: newQueryParams
    }).toString()
    this._location.go(url);

    this.usefullLinksData = [];
    console.log(r);
    this.pageIndex = r.data.page_index;
    this.length = r.data.total;
    for (let i in r.data.items) {
      let usefulLink = r.data.items[i];
      const userClicksLabel = this.prepareUserClicksLabel(usefulLink['user_clicks'])
      this.usefullLinksData.push({
        id: usefulLink['id'],
        link: usefulLink['url'],
        description: usefulLink['description'],
        userFavorites: usefulLink['user_favorites'],
        userClicks: usefulLink['user_clicks'],
        userClicksLabel: userClicksLabel,
        favorite: usefulLink['favorite'],
        rating: 0,
        tags: usefulLink['tags'],
      })
    }
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.usefullLinksData);
  }

  errorUsefulLinksList(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  updatePage() {
    // this._spinner.show();
    this.loadListOftags();
    this._fhq.api().useful_links_list({
      "page_index": this.pageIndex,
      "page_size": this.pageSize,
      "filter": this.searchValue,
      "filter_by_tag": this.filterByTag,
    })
      .done((r: any) => this.successUsefulLinksList(r))
      .fail((err: any) => this.errorUsefulLinksList(err));
  }

  public getServerData(event?: PageEvent){
    console.log(event);

    this.pageIndex = event.pageIndex
    this.pageSize = event.pageSize
    this.updatePage();
    return event;
  }

  prepareUserClicksLabel(n: number) {
    // console.log("prepareUserClicksLabel", n)
    if (n >= 1000 && n < 1000000) {
      // console.log("prepareUserClicksLabel > 999 ", n)
      return Math.floor(n / 1000) + "k";
    } else if (n >= 1000000) {
      return Math.floor(n / 1000000) + "M";
    }
    return "" + n;
  }

  successUsefulLinksOneItem(r: any) {
    this._spinner.hide();
    this.usefullLinksData.forEach((el: any) => {
      if (el.id == r.data.id) {
        el.userFavorites = r.data.user_favorites
        el.userClicks = r.data.user_clicks
        el.userClicksLabel = this.prepareUserClicksLabel(r.data.user_clicks)
        el.favorite = r.data.favorite
      }
    })
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.usefullLinksData);
    this._cdr.detectChanges();
  }

  errorUsefulLinksOneItem(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
    
  }

  updateOneItem(usefulLinkId: number) {
    console.log("updateOneItem", usefulLinkId);
    this._spinner.show();
    this._fhq.api().useful_links_retrieve({
      "useful_link_id": usefulLinkId
    })
      .done((r: any) => this.successUsefulLinksOneItem(r))
      .fail((err: any) => this.errorUsefulLinksOneItem(err));
  }

  successUsefulLinksFavorite(r: any) {
    this.updateOneItem(r.data.useful_link_id)
  }

  errorUsefulLinksFavorite(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
    
  }

  addToFavorite(id: number) {
    console.log("addToFavorite", id );
    if (this._fhq.isAuthorized) {

    }
    this._spinner.show();
    this._fhq.api().useful_links_user_favorite({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinksFavorite(r))
      .fail((err: any) => this.errorUsefulLinksFavorite(err));
  }

  successUsefulLinksUserUnfavorite(r: any) {
    this.updateOneItem(r.data.useful_link_id)
  }

  errorUsefulLinksUserUnfavorite(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  removeFromFavorite(id: number) {
    console.log("removeFromFavorite", id );
    this._spinner.show();
    this._fhq.api().useful_links_user_unfavorite({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinksUserUnfavorite(r))
      .fail((err: any) => this.errorUsefulLinksUserUnfavorite(err));
  }

  successUsefulLinkClicked(r: any) {
    console.log("successUsefulLinkClicked", r);
    this.updateOneItem(r.data.useful_link_id)
  }

  errorUsefulLinkClicked(err: any) {
    console.error("errorUsefulLinkClicked", err);
    this._spinner.hide();
  }

  linkClicked(id: number) {
    // console.log("linkClicked", id );
    this._fhq.api().useful_links_clicked({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinkClicked(r))
      .fail((err: any) => this.errorUsefulLinkClicked(err));
  }

  successLoadListOftags(r: any) {
    // console.log("successLoadListOftags", r);
    this.tagList = [];
    r.data.forEach(el => {
      this.tagList.push({
        name: el.tag,
        counter: el.counter,
      })
    });
    this._cdr.detectChanges();
  }

  errorLoadListOftags(err: any) {
    console.error("errorLoadListOftags", err);
    // this._spinner.hide();
  }

  loadListOftags() {
    this._fhq.api().useful_links_tag_list({
      // nothing yet
    })
      .done((r: any) => this.successLoadListOftags(r))
      .fail((err: any) => this.errorLoadListOftags(err));
  }

  applyFilterByTag(tag: string) {
    if (this.filterByTag == tag) {
      this.filterByTag = "";
    } else {
      this.filterByTag = tag;
    }
    this.updatePage();
  }
}
