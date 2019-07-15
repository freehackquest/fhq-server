import { Component, OnInit, Input, ChangeDetectorRef, NgZone, ViewChild, ElementRef } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { SpinnerService } from '../../services/spinner.service';
import { EmailValidatorService } from '../../email-validator.service';
import { Location } from '@angular/common';

declare var fhq: any;

@Component({
  selector: 'app-modal-dialog-sign-in',
  templateUrl: './modal-dialog-sign-in.component.html',
  styleUrls: ['./modal-dialog-sign-in.component.css']
})
export class ModalDialogSignInComponent implements OnInit {
  errorMessage: string = null;
  @ViewChild('signinEmail', { static: true }) signinEmail : ElementRef;
  @ViewChild('signinPassword', { static: true }) signinPassword : ElementRef;

  constructor(
    public _activeModal: NgbActiveModal,
    private _route: ActivatedRoute,
    private _router: Router,
    private _cdr: ChangeDetectorRef,
    private _zone: NgZone,
    private _spinnerService: SpinnerService,
    private _emailValidator: EmailValidatorService,
  ) { }

  ngOnInit() {

  }

  openResetPasswordPage() {
    this._activeModal.dismiss('Reset password');
    this._zone.run(() => this._router.navigate(['/reset-password'])).then();
  }

  openResgistrationPage() {
    this._activeModal.dismiss('Registration');
    this._zone.run(() => this._router.navigate(['/registration'])).then();
  }

  signInGo() {
    const email = this.signinEmail.nativeElement.value.toLowerCase().trim();
    const password = this.signinPassword.nativeElement.value;
    this._spinnerService.show();
    this.errorMessage = null;
    fhq.login({
      "email": email,
      "password": password,
    })
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this._cdr.detectChanges();
    this._spinnerService.hide();
    this._activeModal.dismiss('SignIn Success');
    // location.reload();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }
}
