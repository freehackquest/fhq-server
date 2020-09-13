import { Component, OnInit, ChangeDetectorRef, NgZone  } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';
import { FhqService } from '../../services/fhq.service';

@Component({
  selector: 'app-ctf-wall',
  templateUrl: './ctf-wall.component.html',
  styleUrls: ['./ctf-wall.component.css']
})
export class CtfWallComponent implements OnInit {

  constructor(
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
    public _fhq: FhqService,
  ) { }

  ngOnInit(): void {
    this._route.params.subscribe( (params) => {
      this._spinnerService.hide();
    });
  }

}
