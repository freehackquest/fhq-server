import { Component, OnInit, Output, EventEmitter, ViewChild, ChangeDetectorRef, ElementRef  } from '@angular/core';
import { FormControl } from '@angular/forms';
import { SpinnerService } from '../../services/spinner.service';
import { FhqService } from '../../services/fhq.service';
import { Router } from '@angular/router';
import { ActivatedRoute } from '@angular/router';
import { Observable }  from 'rxjs/Observable';
import 'rxjs/add/operator/debounceTime';
import 'rxjs/add/operator/throttleTime';
import 'rxjs/add/observable/fromEvent';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-games',
  templateUrl: './games.component.html',
  styleUrls: ['./games.component.css']
})
export class GamesComponent implements OnInit {
  countPages = 0;
  currentPage = 0;
  onPage = 7;
  errorMessage: string = null;
  dataList: Array<any> = [];
  searchValue: String = '';

  constructor(
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _route: ActivatedRoute,
    private _router: Router,
    private _el: ElementRef,
    private _fhq: FhqService,
  ) { }

  ngOnInit() {
    this._route.params.subscribe( (params) => {
      if (!params['id']) {
        this._router.navigate(['/games', 0]);
        return;
      }
      this.currentPage = parseInt(params['id'], 10);
      this.loadData();
    });
  }
  
  loadData() {
    // this.searchTaskControl.value
    const _data = {
      "page": this.currentPage,
      "onpage": this.onPage,
      "search": this.searchValue,
    }
    this._spinnerService.show();
    this._fhq.api().games(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinnerService.hide();
    this.countPages = Math.ceil(parseInt(r.count, 10) / this.onPage);

    this.dataList = []
    r.data.forEach((el: any) => {  
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
