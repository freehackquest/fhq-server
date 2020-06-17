import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute, Router } from '@angular/router';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { FormControl, Validators } from '@angular/forms';

@Component({
  selector: 'app-user-profile',
  templateUrl: './user-profile.component.html',
  styleUrls: ['./user-profile.component.css']
})
export class UserProfileComponent implements OnInit {
  errorMessage: string = null;
  subscription: any;

  userId: number = 0;
  userAbout: string = "";
  userDtCreate: string = "";
  userDtLastLogin: string = "";
  userEmail: string = "";
  userLogo: string = "";
  userNick: string = "";
  userRating: number = 0;
  userRole: string = "";
  userStatus: string = "";
  userUniversity: string = "";
  userUuid: string = "";

  resultOfChangeUserInfo: string = null;

  userNewNickFormControl = new FormControl('', [
    Validators.required
  ]);
  userNewUniversityFormControl = new FormControl('', [
  ]);
  userNewAboutFormControl = new FormControl('', [
  ]);

  constructor(
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
    private _router: Router,
    private _cdr: ChangeDetectorRef,
    private _modalService: NgbModal,
    private _fhq: FhqService
  ) {

  }

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
      this.userId = parseInt(this._fhq.userdata.id, 10);
      this.userAbout = this._fhq.userdata.about;
      this.userDtCreate = this._fhq.userdata.dt_create;
      this.userDtLastLogin = this._fhq.userdata.dt_last_login;
      this.userEmail = this._fhq.userdata.email;
      this.userLogo = this._fhq.userdata.logo;
      this.userNick = this._fhq.userdata.nick;
      this.userRating = this._fhq.userdata.rating;
      this.userRole = this._fhq.userdata.role;
      this.userStatus = this._fhq.userdata.status;
      this.userUniversity = this._fhq.userdata.university;
      this.userUuid = this._fhq.userdata.uuid;
      this.userNewNickFormControl.setValue(this.userNick);
      this.userNewUniversityFormControl.setValue(this.userUniversity);
      this.userNewAboutFormControl.setValue(this.userAbout);
      this._cdr.detectChanges();
    } else {
      this.userId = 0;
    }
    this._spinnerService.hide();
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }

  doChangeUserInfo() {
    this.resultOfChangeUserInfo = null;
    this._cdr.detectChanges();

    const userNewNickValue = this.userNewNickFormControl.value.trim();
    const userNewUniversityValue = this.userNewUniversityFormControl.value.trim();
    const userNewAboutValue = this.userNewAboutFormControl.value.trim();

    this._spinnerService.show();
    this._fhq.api().user_update({
      "nick": userNewNickValue,
      "university": userNewUniversityValue,
      "about": userNewAboutValue,
      "userid": this.userId,
    })
      .done((r: any) => this.successChangedUserInfo(r))
      .fail((err: any) => this.errorChangedUserInfo(err));
  }

  successChangedUserInfo(r: any) {
    // console.log("successResponse: ", r);
    this.userNick = r.data.nick;
    this.userUniversity = r.data.university;
    this.userAbout = r.data.about;
    this._cdr.detectChanges();
    this._spinnerService.hide();
  }

  errorChangedUserInfo(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    this.resultOfChangeUserInfo = err.error;
    this._cdr.detectChanges();
  }

}
