import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-feedback',
  templateUrl: './feedback.component.html',
  styleUrls: ['./feedback.component.css']
})
export class FeedbackComponent implements OnInit {
  errorMessage: string = null;
  subscription: any;
  sendedFeedback: boolean = false;
  @ViewChild('feedbackTarget') feedbackTarget : ElementRef; 
  @ViewChild('feedbackFrom') feedbackFrom : ElementRef;
  @ViewChild('feedbackMessage') feedbackMessage : ElementRef;

  constructor(
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _spinner: SpinnerService,
  ) { }

  ngOnInit() {
    this.subscription = this._fhq.changedState
      .subscribe(() => this._cdr.detectChanges());
    this._spinner.hide();
  }

  ngOnDestroy() {
	  this.subscription.unsubscribe();
  }

  sendFeedback() {
    // this._spinnerService.show();
    this.errorMessage = null;
    const target = this.feedbackTarget.nativeElement.value;
    console.log("target", target);
    const from = this.feedbackFrom.nativeElement.value;
    console.log("from", from);
    const msg = this.feedbackMessage.nativeElement.value;
    console.log("msg", msg);
    this._fhq.api().feedback_add({
      "type": target,
      "from": from,
      "text": msg,
      })
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
	  // console.log("successResponse: ", r);
    this.sendedFeedback = true;
    this._spinner.hide();
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }
  
  sendNewFeedback() {
    this.sendedFeedback = false;
    this._cdr.detectChanges();
  }
}