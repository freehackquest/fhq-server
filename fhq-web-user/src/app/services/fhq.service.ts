import { Injectable, EventEmitter, Inject } from '@angular/core';
import { PlatformLocation } from '@angular/common';
import { ToastrService } from 'ngx-toastr';

declare var fhq: any;

@Injectable({
  providedIn: 'root'
})
export class FhqService {
  isAuthorized: boolean = false;
  connectionState: string = '';
  serverHost: string = 'some';
  currentProtocol: string = 'http:';
  userdata: any = {};
  changedState = new EventEmitter<any>();

  constructor(
    private _toastr: ToastrService,
    private _location: PlatformLocation,
  ) {
    this.serverHost = this._location.hostname;
    this.currentProtocol = this._location.protocol;

    fhq.bind('connected', () => this.wsConnected() );
    fhq.bind('disconnected', () => this.connectToServer() );
    fhq.bind('broken', () => this.wsBroken() );
    fhq.bind('reconnecting', () => this.wsReconnecting() );

    fhq.bind('userdata', (data: any) => this.updateUserData(data));
    fhq.bind('notify', (data: any) => this.showNotification(data));
  }

  showNotification(data: any) {
    console.log(data);

    if (data.type === 'info') {
      this._toastr.info(data.message)
    } else if (data.type === 'success') {
      this._toastr.success(data.message)
    } else if (data.type === 'danger') {
      this._toastr.error(data.message)
    } else if (data.type === 'warning') {
      this._toastr.warning(data.message)
    } else {
      // unknown type of message
      this._toastr.warning(data.message)
    }
  }

  updateUserData(data: any) {
    console.log("updateUserData: ", data);
    this.isAuthorized = data.nick != undefined;
    this.userdata = data;
    this.changedState.emit();
  }

  connectToServer() {
    console.log("connectToServer");
    if (this.connectionState == 'OK') {
      this.connectionState = 'BROKEN';
    } else {
      this.connectionState = 'WAIT';
    }

    let baseUrl = 'ws://' + this.serverHost + ':1234/api-ws/';
    if (this.currentProtocol == "https:") {
      baseUrl = 'wss://' + this.serverHost + ':4613/api-wss/';
    }

    if (this.serverHost == 'freehackquest.com') {
      baseUrl = 'wss://freehackquest.com/api-wss/';
    }
    // baseUrl = 'wss://freehackquest.com/api-wss/';
    // baseUrl = 'ws://freehackquest.com/api-ws/';

    fhq.init({'baseUrl': baseUrl});
  }

  wsConnected() {
    this.connectionState = 'OK';
    this.changedState.emit();
  }

  wsBroken() {
    this.connectionState = 'BROKEN';
    this.changedState.emit();
  }

  wsReconnecting() {
    this.connectionState = 'RECONN';
    this.changedState.emit();
  }

  logout() {
    fhq.deinit();
    location.reload();
  }

  api() {
    return fhq;
  }
}
