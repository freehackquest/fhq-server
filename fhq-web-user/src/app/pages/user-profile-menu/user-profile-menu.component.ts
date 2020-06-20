import { Component, OnInit } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { Router, ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-user-profile-menu',
  templateUrl: './user-profile-menu.component.html',
  styleUrls: ['./user-profile-menu.component.css']
})
export class UserProfileMenuComponent implements OnInit {
  subscription: any;
  userRole: String = "";
  urlPath:  String = "";
  constructor(
    private _fhq: FhqService,
    private _router: Router,
    private _activatedRoute: ActivatedRoute,
  ) { }

  ngOnInit() {
    this.updatePage();
    this.subscription = this._fhq.changedState
    .subscribe(() => this.updatePage());

    this.urlPath = this._activatedRoute.snapshot.url[0].path
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

  userSignout() {
    this._fhq.logout();
  }
}
