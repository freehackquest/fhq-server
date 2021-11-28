import { Component, OnInit, Output, EventEmitter, ChangeDetectorRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { Router, ActivatedRoute } from '@angular/router';
import { debounceTime, distinctUntilChanged } from 'rxjs/operators';
import { escape } from 'lodash';
import { FhqService } from '../../services/fhq.service';
import { FormControl, Validators } from '@angular/forms';
import { PageEvent } from '@angular/material/paginator';
import { Location } from '@angular/common';

declare var _: any;

@Component({
  selector: 'app-events',
  templateUrl: './events.component.html',
  styleUrls: ['./events.component.css']
})
export class EventsComponent implements OnInit {
  @Output() loading = new EventEmitter<boolean>();
  searchValue: string = '';
  searchControl = new FormControl('');
  // formCtrlSub = new Subject<string>();

  pageEvent: PageEvent;
  pageIndex: number = 0;
  pageSize: number = 7;
  length: number = 0;
  pageSizeOptions = [7, 10, 25, 50];
  errorMessage: string = null;
  dataList: Array<any> = [];
  subscriptionOnNotify: any = null;

  emailFormControl = new FormControl('', [
    Validators.required,
    Validators.email,
  ]);

  constructor(
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _router: Router,
    private _fhq: FhqService,
    private _location: Location,
    private _activatedRoute: ActivatedRoute,
  ) { }

  onSearchBoxValueChange() {
    this.pageIndex = 0;
  }

  ngOnInit() {
    this._activatedRoute.queryParams.subscribe(params => {
      if (params["search"]) {
        this.searchValue = params["search"];
      }

      if (params["page_index"]) {
        this.pageIndex = parseInt(params["page_index"],0);
      }

      if (params["page_size"]) {
        this.pageSize = parseInt(params["page_size"],0);
      }
      /*this.subscription = this._fhq.changedState
        .subscribe(() => this.updatePage());*/
      this.loadData();
    });

    this.searchControl.valueChanges
      .pipe(
        debounceTime(1000),
        distinctUntilChanged(),
      )
      .subscribe((newValue): void => {
        this.searchValue = newValue;
        this.pageIndex = 0;
        console.log(newValue);
        this.loadData();
      });

    this.subscriptionOnNotify = this._fhq.onNotify
      .subscribe(() => this.loadData());
  }
  
  ngOnDestroy() {
    if (this.subscriptionOnNotify != null) {
      this.subscriptionOnNotify.unsubscribe();
    }
  }

  public getServerData(event?: PageEvent){
    console.log(event);

    this.pageIndex = event.pageIndex
    this.pageSize = event.pageSize
    this.loadData();
    return event;
  }

  loadData() {
    // this.searchTaskControl.value
    const _data = {
      "page": this.pageIndex,
      "onpage": this.pageSize,
      "search": this.searchValue,
    }
    this._spinnerService.show();
    this._fhq.api().publiceventslist(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    let newQueryParams = {}
    if (this.searchValue !== "") {
      newQueryParams["search"] = this.searchValue;
    }
    if (this.pageIndex !== 0) {
      newQueryParams["page_index"] = this.pageIndex;
    }
    if (this.pageSize !== this.pageSizeOptions[0]) {
      newQueryParams["page_size"] = this.pageSize;
    }
    
    const url = this._router.createUrlTree([], {
      relativeTo: this._activatedRoute, 
      queryParams: newQueryParams
    }).toString()
    this._location.go(url);

    this._spinnerService.hide();
    this.length = r.count
    this.dataList = []
    r.data.forEach((el: any) => {
      el.dt_formated = new Date(el.dt + "Z")
      el["links"] = [];
      el.html_message = escape(el.message)
      // el['html_message'] = el['html_message'].replace(/\[user#(\d+)\]/g, '<a href="/user/$1">[user#$1]</a>')
      el.html_message = el.html_message.replace(/\[quest#(\d+)\][ ]+\(([\w\u0430-\u044f -]+)\)/ig, 'quest <a href="../quest/$1" class="badge badge-light" target="_blank">$2</a>')
      if (el.meta) {
        if (el.meta["useful_link"]) {
          const ul = el.meta["useful_link"];
          if (ul["action"] === "added" || ul["action"] === "updated") {
            el["links"].push({
              "routeLink": "/useful-links/" + ul["id"],
              "text": ul["url"]
            });
          }
        }

        if (el.meta["quest"]) {
          const quest = el.meta["quest"];
          if (quest["action"] === "added" || quest["action"] === "updated" || quest["action"] === "passed") {
            el["links"].push({
              "routeLink": "/quest/" + quest["id"],
              "text": quest["name"]
            });
          }
        }

        if (el.meta["user"]) {
          const u = el.meta["user"];
          if (u["action"] === "added" || u["action"] === "updated") {
            el["links"].push({
              "routeLink": "/user/" + u["id"],
              "text": u["nick"]
            });
          }
        }
      }
      this.dataList.push(el);
    });
    console.log(this.dataList);
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }

}
