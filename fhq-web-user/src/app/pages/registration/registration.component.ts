import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { TranslationService } from 'angular-l10n';
import { SpinnerService } from '../../services/spinner.service';
import { EmailValidatorService } from '../../email-validator.service';

declare var fhq: any;

@Component({
  selector: 'app-registration',
  templateUrl: './registration.component.html',
  styleUrls: ['./registration.component.css']
})
export class RegistrationComponent implements OnInit {
  errorMessage: string = null;
  successRegistered: boolean = false;
  @ViewChild('regEmail', { static: false }) regEmail : ElementRef;
  @ViewChild('regUniversity', { static: false }) regUniversity: ElementRef;

  constructor(
    public _translation: TranslationService,
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _emailValidator: EmailValidatorService,
  ) { }
 
  ngOnInit() {
    this._spinnerService.hide();
  }

  registryTry() {
    this.errorMessage = null;
    // this._cdr.detectChanges();
    const email = this.regEmail.nativeElement.value.toLowerCase().trim();
    if (email == '') {
      this.errorMessage = 'E-mail required';
      this._cdr.detectChanges();
      return;
    }
    const r = this._emailValidator.checkEmail(email);
    if (!r.result) {
      this.errorMessage = r.error;
      this._cdr.detectChanges();
      return;
    }
    
    const university = this.regUniversity.nativeElement.value.toLowerCase().trim();
    
    this._spinnerService.show();
    fhq.registration({
      "email": email,
      "university": university
    })
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this.successRegistered = true;
    this._cdr.detectChanges();
    this._spinnerService.hide();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }
}
