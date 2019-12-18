import { Component, OnInit } from '@angular/core';
import { FhqService } from '../../services/fhq.service';

@Component({
  selector: 'app-user-profile-menu',
  templateUrl: './user-profile-menu.component.html',
  styleUrls: ['./user-profile-menu.component.css']
})
export class UserProfileMenuComponent implements OnInit {
  subscription: any;
  userRole: String = "";

  constructor(
    private _fhq: FhqService
  ) { }

  ngOnInit() {
    this.updatePage();
    this.subscription = this._fhq.changedState
      .subscribe(() => this.updatePage());
  }

  ngOnDestroy() {
    this.subscription.unsubscribe();
  }

  updatePage() {
    if (this._fhq.isAuthorized) {
      this.userRole = this._fhq.userdata.role;
    } else {
      this.userRole = ''
    }
  }

}
