import { Component, OnInit, ChangeDetectorRef, NgZone } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { FhqService } from '../../services/fhq.service';

@Component({
  selector: 'ctf-attack-defence',
  templateUrl: './ctf-attack-defence.component.html',
  styleUrls: ['./ctf-attack-defence.component.css']
})
export class CtfAttackDefenceComponent implements OnInit {

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
