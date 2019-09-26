import { Component, OnInit, ChangeDetectorRef, NgZone } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { FhqService } from '../../services/fhq.service';

@Component({
  selector: 'app-fhq-jury-ad',
  templateUrl: './fhq-jury-ad.component.html',
  styleUrls: ['./fhq-jury-ad.component.css']
})
export class FhqJuryAdComponent implements OnInit {

  constructor(
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
    private _router: Router,
    private _cdr: ChangeDetectorRef,
    private _zone: NgZone,
    public _fhq: FhqService,
  ) { }

  ngOnInit() {
    this._route.params.subscribe( (params) => {
      this._spinnerService.hide();
    });
  }

}
