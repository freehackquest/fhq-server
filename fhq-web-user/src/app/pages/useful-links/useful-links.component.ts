import { Component, OnInit, ChangeDetectorRef, ViewChild } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FormControl } from '@angular/forms';
import { Subscription } from 'rxjs';
import { Router } from '@angular/router';
import { PageEvent } from '@angular/material';
import { MatTableDataSource } from '@angular/material';
import { FhqService } from '../../services/fhq.service';

export interface UsefulLinkElement {
  id: number;
  link: string;
  description: string;
  rating: number;
  tags: Array<string>;
}

@Component({
  selector: 'app-useful-links',
  templateUrl: './useful-links.component.html',
  styleUrls: ['./useful-links.component.css']
})
export class UsefulLinksComponent implements OnInit {
  public usefullLinksData: UsefulLinkElement[] = [];
  
  pageEvent: PageEvent;
  pageIndex: number = 0;
  pageSize: number = 7;
  length: number = 0;
  pageSizeOptions = [7, 10, 25, 50];
  errorMessage: string = null;

  filteredUsefullLinksData: UsefulLinkElement[] 
  searchValue: String = '';
  searchControl = new FormControl('');
  formCtrlSub: Subscription;

  dataSource = new MatTableDataSource<UsefulLinkElement>();
  displayedColumns: string[] = ['idUsefulLink', 'usefulLinkData'];

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _router: Router,
    private _fhq: FhqService,
  ) { }

  ngOnInit() {
    this.updatePage();

    this._spinner.hide();
    this.formCtrlSub = this.searchControl.valueChanges
    .debounceTime(1000)
    .subscribe((newValue) => {
      this.searchValue = newValue
      this.applyFilter();
    });
    this.applyFilter();
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.usefullLinksData);
  }

  successUsefulLinksList(r: any) {
    this._spinner.hide();
    // this.usefullLinksData = [];
    for (let i in r.data) {
      let usefulLink = r.data[i];
      this.usefullLinksData.push({
        id: usefulLink['id'],
        link: usefulLink['url'],
        description: usefulLink['description'],
        rating: 0,
        tags: [],
      })
    }
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.usefullLinksData);
    this._cdr.detectChanges();
  }

  errorUsefulLinksList(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  updatePage() {
    this._spinner.show();
    this._fhq.api().useful_links_list({})
      .done((r: any) => this.successUsefulLinksList(r))
      .fail((err: any) => this.errorUsefulLinksList(err));
  }

  applyFilter() {
    const _sv = this.searchValue.toUpperCase();
    console.log(_sv);
    this.filteredUsefullLinksData = []
    this.usefullLinksData.forEach((el: any) => {
      var filteredLink = {
        link: el.link,
        description: el.description
      }
      if (el.link.toUpperCase().indexOf(_sv) !== -1
        || el.description.toUpperCase().indexOf(_sv) !== -1) {
          this.filteredUsefullLinksData.push({
            id: el.id,
            link: el.link,
            description: el.description,
            rating: el.rating,
            tags: el.tags
          })
      }
    });
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.filteredUsefullLinksData);
  }

  openLink(link) {
    console.log(link);
  }
}
