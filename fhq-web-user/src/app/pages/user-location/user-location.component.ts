import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-user-location',
  templateUrl: './user-location.component.html',
  styleUrls: ['./user-location.component.css']
})
export class UserLocationComponent implements OnInit {
  subscription: any;
  userId: number = 0;
  userLastIp: string = "";
  userCountry: string = "";
  userRegion: string = "";
  userCity: string = "";

  constructor(
    private _fhq: FhqService,
    private _cdr: ChangeDetectorRef,
    private _spinnerService: SpinnerService,
  ) { }

  ngOnInit() {
    this.updatePage();
    this.subscription = this._fhq.changedState
      .subscribe(() => this.updatePage());
  }

  updatePage() {
    if (this._fhq.isAuthorized) {
      this.userId = parseInt(this._fhq.userdata.id, 10);
      this.userLastIp = this._fhq.userdata.last_ip;
      this.userCountry = this._fhq.userdata.country;
      this.userRegion = this._fhq.userdata.region;
      this.userCity = this._fhq.userdata.city;
      this._cdr.detectChanges();
    } else {
      this.userId = 0;
    }
    this._spinnerService.hide();
  }
}
