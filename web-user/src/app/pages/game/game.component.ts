import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FhqService } from 'src/app/services/fhq.service';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'app-game',
  templateUrl: './game.component.html',
  styleUrls: ['./game.component.css']
})
export class GameComponent implements OnInit {

  gameuuid: string;
  errorMessage: string = null;
  subscription: any = null;

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    public _fhq: FhqService,
    private _route: ActivatedRoute,
    private _router: Router,
  ) { }

  ngOnInit(): void {
    this._route.params.subscribe( (params) => {
      if (!params['id']) {
        this._router.navigate(['/games']);
        return;
      }
      this.gameuuid = params['uuid'];
      // TODO check possible subjects
      this.loadData();

      this.subscription = this._fhq.changedState
        .subscribe(() => this.loadData());
    });
  }

  loadData() {
    const _data = {
      uuid: this.gameuuid
    }
    this._spinner.show();
    this._fhq.api().game_info(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinner.hide();
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    this._spinner.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }
}
