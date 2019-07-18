import { Component, OnInit, ChangeDetectorRef, Inject } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { PlatformLocation } from '@angular/common';

declare var fhq: any;

@Component({
  selector: 'app-server-api',
  templateUrl: './server-api.component.html',
  styleUrls: ['./server-api.component.css']
})
export class ServerApiComponent implements OnInit {
  apiVersion: String = '';
  apisList: Array<any> = [];
  errorMessage: String = null;
  serverWsPort: number = -1;
  serverWssPort: number = -1;
  serverHost: string = 'some';
  currentProtocol: string = 'http:';
  constructor(
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _location: PlatformLocation,
  ) {
    this.serverHost = this._location.hostname;
    this.currentProtocol = this._location.protocol;
  }

  ngOnInit() {
    const _data = {}
    this._spinnerService.show();
    fhq.server_api(_data)
    .done((r: any) => this.successResponse(r))
    .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinnerService.hide();
    this.apiVersion = r.version;
    if (r.server_ws_port) {
      this.serverWsPort = r.server_ws_port;
    }
    if (r.server_wss_port > 0) {
      this.serverWssPort = r.server_wss_port;
    }

    this.apisList = []
    r.data.forEach((el: any) => {
      this.apisList.push(el);
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
