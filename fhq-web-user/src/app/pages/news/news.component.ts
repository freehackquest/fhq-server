import { Component, OnInit, Output, EventEmitter, ViewChild, ChangeDetectorRef, ElementRef } from '@angular/core';
import { FormControl } from '@angular/forms';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute, Router } from '@angular/router';
import { Observable }  from 'rxjs/Observable';
import 'rxjs/add/operator/debounceTime';
import 'rxjs/add/operator/throttleTime';
import 'rxjs/add/observable/fromEvent';
import { Subscription } from 'rxjs';

declare var fhq: any;

@Component({
  selector: 'app-news',
  templateUrl: './news.component.html',
  styleUrls: ['./news.component.css']
})
export class NewsComponent implements OnInit {
  @Output() loading = new EventEmitter<boolean>();
  searchValue: String = '';
  private searchControl = new FormControl('');
  formCtrlSub: Subscription;

  countPages = 0;
  currentPage = 0;
  onPage = 7;
  errorMessage: string = null;
  dataList: Array<any> = [];

  constructor(
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _route: ActivatedRoute,
    private _router: Router,
    // private _el: ElementRef,
  ) { }

  onSearchBoxValueChange() {
    this.currentPage = 1;
}

  ngOnInit() {
    this._route.params.subscribe( (params) => {
      if (!params['id']) {
        this._router.navigate(['/news', 0]);
        return;
      }
      this.currentPage = parseInt(params['id'], 10);
      this.loadData();
    });

    // debounce keystroke events
    this.formCtrlSub = this.searchControl.valueChanges
      .debounceTime(1000)
      .subscribe((newValue) => {
        this.searchValue = newValue
        this.currentPage = 0;
        console.log(newValue);
        this.loadData();
      });
  }
  
  prevPage() {
    this.currentPage--;
    this.loadData();
  }

  nextPage() {
    this.currentPage++;
    this.loadData();
  }

  loadData() {
    // this.searchTaskControl.value
    const _data = {
      "page": this.currentPage,
      "onpage": this.onPage,
      "search": this.searchValue,
    }
    this._spinnerService.show();
    fhq.publiceventslist(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinnerService.hide();
    this.countPages = Math.ceil(parseInt(r.count, 10) / this.onPage);

    this.dataList = []
    r.data.forEach((el: any) => {
      el['html_message'] = el.message;
      el['html_message'] = el['html_message'].replace(/\[user#(\d+)\]/g, '<a href="/user/$1">[user#$1]</a>')
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
