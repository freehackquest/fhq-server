import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { TranslationService } from 'angular-l10n';
import { SpinnerService } from '../../services/spinner.service';
import { EmailValidatorService } from '../../email-validator.service';

declare var fhq: any;

@Component({
  selector: 'app-reset-password',
  templateUrl: './reset-password.component.html',
  styleUrls: ['./reset-password.component.css']
})
export class ResetPasswordComponent implements OnInit {
  errorMessage: string = null;
  successReseted: boolean = false;
  @ViewChild('resetPasswordEmail', { static: false }) resetPasswordEmail : ElementRef;

  constructor(
    public _translation: TranslationService,
    private _spinnerService: SpinnerService,
	private _cdr: ChangeDetectorRef,
	private _emailValidator: EmailValidatorService,
  ) { }

  ngOnInit() {
	this._spinnerService.hide();
  }

  resetPasswordTry() {
    this.errorMessage = null;
    this._cdr.detectChanges();

    const email = this.resetPasswordEmail.nativeElement.value.toLowerCase().trim();
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
		
    this._spinnerService.show();
    fhq.user_reset_password({
      "email": email,
    })
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this.successReseted = true;
    this._cdr.detectChanges();
    this._spinnerService.hide();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this.successReseted = false;
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }

}

/*

fhq.ui.user_reset_password = function() {
	$('#user_reset_password_error').html('');
	var data = {};
	data.email = $('#user_reset_password_email').val();
	fhq.ui.showLoading();
	fhq.ws.user_reset_password(data).done(function(r){
		fhq.ui.hideLoading();
		$('#content_page').html('Please check your mailbox (also look in spam)');
	}).fail(function(r){
		console.error(r);
		$('#user_reset_password_error').html(fhq.t(r.error));
		fhq.ui.hideLoading();
	})
		
}

fhq.ui.loadResetPasswordPage = function() {
	fhq.ui.hideLoading();
	fhq.changeLocationState({'user_reset_password':''});
	$('#content_page').html('');
	
	$('#content_page').append(''
		+ '	<div class="form-group row">'
		+ ' 	<div class="col-sm-4"></div>'
		+ ' 	<div class="col-sm-4">'
		+ '			<h1 class="text-center">' + fhq.t('Reset password') + '</h1>'
		+ '		</div>'
		+ ' 	<div class="col-sm-4"></div>'
		+ '	</div>'
		+ '	<div class="form-group row">'
		+ ' 	<div class="col-sm-4"></div>'
		+ ' 	<div class="col-sm-4">'
		+ '			<label for="user_reset_password_email" class="col-form-label">E-mail (required):</label>'
		+ '			<input type="email" placeholder="your@email.com" class="form-control" value="" id="user_reset_password_email"/>'
		+ '		</div>'
		+ ' 	<div class="col-sm-4"></div>'
		+ '	</div>'
		+ '	<div class="form-group row">'
		+ ' 	<div class="col-sm-4"></div>'
		+ ' 	<div class="col-sm-4 text-center">'
		+ '			<div class="btn btn-success" onclick="fhq.ui.user_reset_password();">' + fhq.t('Reset') + '</div>'
		+ '		</div>'
		+ ' 	<div class="col-sm-4"></div>'
		+ '	</div>'
		+ '	<div class="form-group row">'
		+ ' 	<div class="col-sm-4"></div>'
		+ ' 	<div class="col-sm-4 text-center" id="user_reset_password_error">'
		+ '		</div>'
		+ ' 	<div class="col-sm-4"></div>'
		+ '	</div>'
	);
}

*/