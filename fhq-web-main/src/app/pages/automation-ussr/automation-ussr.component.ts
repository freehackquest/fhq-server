import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-automation-ussr',
  templateUrl: './automation-ussr.component.html',
  styleUrls: ['./automation-ussr.component.css']
})

export class AutomationUssrComponent implements OnInit {
  resultOfregistration : string = "";
  errorResultOfregistration : string = "";
  @ViewChild('regUsername', { static: true }) regUsername : ElementRef;
  @ViewChild('afToken', { static: true }) afToken : ElementRef;
  @ViewChild('afFlag', { static: true }) afFlag : ElementRef;
  resultOfSendFlag : string = "";
  errorResultOfSendFlag : string = "";

  constructor(
    private _http: HttpClient,
    private _cdr: ChangeDetectorRef,
    private _spinner: SpinnerService,
  ) { }

  ngOnInit() {
    this._spinner.hide();
  }

  requestRegistration() {
    let username = this.regUsername.nativeElement.value;
    username = encodeURIComponent(username);
    this._http.get("https://freehackquest.com/automation-ussr/api/v1/signup/?username=" + username).subscribe(
      (data: any) => {
        this.resultOfregistration = data;
        console.log(data);
        this._cdr.detectChanges();
      },
      (err: any) => {
        console.error(err);
        this.errorResultOfregistration = err.error.text
      }
    );
  }

  sendFlag() {
    let token = this.afToken.nativeElement.value;
    token = encodeURIComponent(token);
    let flag = this.afFlag.nativeElement.value;
    flag = encodeURIComponent(flag);
    // token = "LgugYouIgP";

    this._http.get("https://freehackquest.com/automation-ussr/api/v1/flag/?token=" + token + "&flag=" + flag).subscribe(
      (data: any) => {
        this.resultOfSendFlag = data;
        console.log(data);
        this._cdr.detectChanges();
      },
      (err: any) => {
        console.error(err);
        this.errorResultOfSendFlag = err.error.text
      }
    );
  }

}
