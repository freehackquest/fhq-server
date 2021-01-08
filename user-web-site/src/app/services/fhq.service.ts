import { Injectable, EventEmitter } from '@angular/core';
import { PlatformLocation } from '@angular/common';
import { MatSnackBar } from '@angular/material/snack-bar';

declare var fhq: any;
declare var fhqconfig: any;

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
  onNotify = new EventEmitter<any>();

  constructor(
    private _snackBar: MatSnackBar,
    private _location: PlatformLocation,
  ) {
    this.serverHost = this._location.hostname;
    // this.serverHost = 'freehackquest.com';
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

    var message: string = '';
    var action: string = '';
    if (data.type === 'info') {
      message = "Info: " + data.message
    } else if (data.type === 'success') {
      message = "Success: " + data.message
    } else if (data.type === 'danger') {
      message = "Danger: " + data.message
    } else if (data.type === 'warning') {
      message = "Warning: " + data.message
    } else {
      // unknown type of message
      message = "Warning: " + data.message
    }
    this._snackBar.open(message, action, {
      duration: 3000,
      panelClass: ['blue-snackbar']
    });

    this.onNotify.emit();
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
    
    let baseUrl = fhqconfig.base_url;
    //if (this.currentProtocol == "https:") {
    //  baseUrl = 'wss://' + this.serverHost + ':4613/api-wss/';
    //}

    //if (this.serverHost == 'freehackquest.com') {
    //  baseUrl = 'wss://freehackquest.com/api-wss/';
    //}
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
