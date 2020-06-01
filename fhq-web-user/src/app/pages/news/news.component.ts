import { Component, OnInit, Output, EventEmitter, ViewChild, ChangeDetectorRef, ElementRef, SecurityContext } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute, Router } from '@angular/router';
import 'rxjs/add/operator/debounceTime';
import 'rxjs/add/operator/throttleTime';
import 'rxjs/add/observable/fromEvent';
import { Subscription } from 'rxjs';
import { DomSanitizer } from '@angular/platform-browser';
import { escape } from 'lodash';
import { FhqService } from '../../services/fhq.service';
import { FormControl, Validators } from '@angular/forms';
import { PageEvent } from '@angular/material';

declare var _: any;

@Component({
  selector: 'app-news',
  templateUrl: './news.component.html',
  styleUrls: ['./news.component.css']
})
export class NewsComponent implements OnInit {
  @Output() loading = new EventEmitter<boolean>();
  searchValue: String = '';
  searchControl = new FormControl('');
  formCtrlSub: Subscription;

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
    private _route: ActivatedRoute,
    private _router: Router,
    private _sanitized: DomSanitizer,
    private _fhq: FhqService,
    // private _el: ElementRef,
  ) { }

  onSearchBoxValueChange() {
    this.pageIndex = 1;
  }

  ngOnInit() {
    this._route.params.subscribe( (params) => {
      if (!params['id']) {
        this._router.navigate(['/news', 0]);
        return;
      }
      this.pageIndex = parseInt(params['id'], 10);
      this.loadData();
    });

    // debounce keystroke events
    this.formCtrlSub = this.searchControl.valueChanges
      .debounceTime(1000)
      .subscribe((newValue) => {
        this.searchValue = newValue
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
    this._spinnerService.hide();
    this.length = r.count
    this.dataList = []
    r.data.forEach((el: any) => {
      el.dt_formated = new Date(el.dt + "Z")
      el.html_message = escape(el.message)
      // el['html_message'] = el['html_message'].replace(/\[user#(\d+)\]/g, '<a href="/user/$1">[user#$1]</a>')
      el.html_message = el.html_message.replace(/\[quest#(\d+)\][ ]+\(([\w\u0430-\u044f -]+)\)/ig, 'quest <a href="../quest/$1" class="badge badge-light" target="_blank">$2</a>')
      this.dataList.push(el);
    });
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }

}
