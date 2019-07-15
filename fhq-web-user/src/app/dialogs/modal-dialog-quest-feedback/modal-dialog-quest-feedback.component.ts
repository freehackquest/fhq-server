import { Component, OnInit, Input, ChangeDetectorRef, NgZone, ViewChild, ElementRef } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { SpinnerService } from '../../services/spinner.service';
import { EmailValidatorService } from '../../email-validator.service';
import { FhqService } from '../../services/fhq.service';


@Component({
  selector: 'app-modal-dialog-quest-feedback',
  templateUrl: './modal-dialog-quest-feedback.component.html',
  styleUrls: ['./modal-dialog-quest-feedback.component.css']
})
export class ModalDialogQuestFeedbackComponent implements OnInit {
  errorMessage: string = null;
  successSended: boolean = false;
  public questName: string = "";
  public questId: string = "";
  public questUrl: string = "";
  @ViewChild('feedbackEmail', { static: true }) feedbackEmail : ElementRef;
  @ViewChild('feedbackText', { static: true }) feedbackText : ElementRef;

  constructor(
    public _activeModal: NgbActiveModal,
    private _cdr: ChangeDetectorRef,
    private _spinnerService: SpinnerService,
    private _emailValidator: EmailValidatorService,
    private _fhq: FhqService,
  ) { }

  ngOnInit() {
  }

  sendQuestFeedback() {
    this.errorMessage = null;
    const email = this.feedbackEmail.nativeElement.value.toLowerCase().trim();
    const text = `[quest#${this.questId}] ${this.questName}\nURL: ${this.questUrl}\n\n` + this.feedbackText.nativeElement.value;

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
    this.errorMessage = null;
    this._fhq.api().feedback_add({
      "type": "error",
      "from": email,
      "text": text,
    })
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this.successSended = true;
    this._cdr.detectChanges();
    this._spinnerService.hide();
    this._activeModal.dismiss('Feedback Success');
    // location.reload();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }

}
