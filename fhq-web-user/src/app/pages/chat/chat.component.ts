import { Component, OnInit, OnDestroy, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FhqService } from '../../services/fhq.service';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';

declare var fhq: any;

@Component({
  selector: 'app-chat',
  templateUrl: './chat.component.html',
  styleUrls: ['./chat.component.css']
})
export class ChatComponent implements OnInit, OnDestroy {
  dataList: Array<any> = [];
  errorMessage: string = null;
  subscription: any;
  @ViewChild('yourMessage', { static: false }) yourMessage : ElementRef;
  @ViewChild('chatMessages', { static: true }) private chatMessages: ElementRef;
  
  constructor(
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    public _fhq: FhqService,
    private _modalService: NgbModal,
  ) { }

  ngOnInit() {
    this.loadData();
    fhq.bind('chat', (m: any) => this.incomeChatMessage(m));
    this.subscription = this._fhq.changedState.subscribe(() => this.loadData());
  }

  ngOnDestroy() {
    console.log('ChatComponent:OnDestroy');
    fhq.unbind('chat'); // will be all unibinded
    this.subscription.unsubscribe();
  }

  loadData() {
    const _data = {}
    this._spinnerService.show();
    fhq.chat_latest_messages(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
    // 
  }

  sendMessage() {
    const msg = this.yourMessage.nativeElement.value;
    this.errorMessage = null;
    console.log(msg);
    var self = this
    fhq.chat_send_message({
      "type": "chat",
      "message": msg
    }).done(function(r: any) {
      console.log('Success: ', r);
    }).fail(function(err: any){
      self.errorMessage = err.error
      console.error('Error:', err);
      self._cdr.detectChanges();
    });
    this.yourMessage.nativeElement.value = '';
  }

  successResponse(r: any) {
    console.log(r);
    this._spinnerService.hide();
    this.dataList = []
    r.data.forEach((el: any) => {
      this.dataList.push(el);
    });
    this.dataList.reverse();
    this._cdr.detectChanges();
    this.chatScrollToBottom();
  }

  chatScrollToBottom() {
    try {
      this.chatMessages.nativeElement.scrollTop = this.chatMessages.nativeElement.scrollHeight;
    } catch(err) { }   
  }

  errorResponse(err: any) {
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }

  incomeChatMessage(m: any) {
    this.dataList.push(m);
    this._cdr.detectChanges();
    this.chatScrollToBottom();
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }
}
