import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';

@Component({
  selector: 'app-quest-proposal',
  templateUrl: './quest-proposal.component.html',
  styleUrls: ['./quest-proposal.component.css']
})
export class QuestProposalComponent implements OnInit {
  questContent: string = "";
  errorMessage: string = null;
  subscription: any;
  listGames: Array<any> = [];
  successSended: boolean = false;

  @ViewChild('gameId', { static: true }) gameId : ElementRef; 
  @ViewChild('questName', { static: true }) questName : ElementRef;
  @ViewChild('questSubject', { static: true }) questSubject : ElementRef;
  @ViewChild('questScore', { static: true }) questScore : ElementRef;
  @ViewChild('questAnswerFormat', { static: true }) questAnswerFormat : ElementRef;
  @ViewChild('questAnswer', { static: true }) questAnswer : ElementRef;
  @ViewChild('questAuthor', { static: true }) questAuthor : ElementRef;
  @ViewChild('questCopyright', { static: true }) questCopyright : ElementRef;
  
  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _modalService: NgbModal,
  ) { }

  ngOnInit() {
    this.subscription = this._fhq.changedState
      .subscribe(() => this._cdr.detectChanges());
    
    this.loadListGames();
  }

  ngOnDestroy() {
	  this.subscription.unsubscribe();
  }

  loadListGames() {
    this._spinner.show();
    this.listGames = [];
    this._fhq.api().games()
      .done((r: any) => this.successListGames(r))
      .fail((err: any) => this.errorListGames(err));
  }

  successListGames(r: any) {
    console.log("successResponse: ", r);
    r.data.forEach((g: any) => {
      this.listGames.push(g);
    });
    this._cdr.detectChanges();
    this._spinner.hide();
  }

  errorListGames(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangeUserInfo = err.error;
    this._cdr.detectChanges();
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }

  tryNewQuest() {
    this.errorMessage = null;
    this.successSended = false;
    this.questContent = "";
    this.loadListGames();
  }

  sendQuestProposal() {
    this.errorMessage = null;
    this.successSended = false;
    let _data = {};
    _data["gameid"] = parseInt(this.gameId.nativeElement.value, 10);
    _data["name"] = this.questName.nativeElement.value.trim();
    _data["subject"] = this.questSubject.nativeElement.value.trim();
    _data["score"] = parseInt(this.questScore.nativeElement.value,10);
    _data["answer_format"] = this.questAnswerFormat.nativeElement.value.trim();
    _data["answer"] = this.questAnswer.nativeElement.value.trim();
    _data["author"] = this.questAuthor.nativeElement.value.trim();
    _data["copyright"] = this.questCopyright.nativeElement.value.trim();
    _data["text"] = this.questContent.trim();
    
    if (_data["name"] === "") {
      this.errorMessage = "Name must be no empty";
      this._cdr.detectChanges()
      return;
    }

    if (_data["text"] === "") {
      this.errorMessage = "Field text must be no empty";
      this._cdr.detectChanges()
      return;
    }

    if (isNaN(_data["score"]) || _data["score"] <= 0) {
      this.errorMessage = "score must be >= 0";
      this._cdr.detectChanges()
      return;
    }

    if (_data["answer_format"] === "") {
      this.errorMessage = "Field answer_format must be no empty";
      this._cdr.detectChanges()
      return;
    }

    if (_data["answer"] === "") {
      this.errorMessage = "Field answer must be no empty";
      this._cdr.detectChanges()
      return;
    }

    if (_data["author"] === "") {
      this.errorMessage = "Field author must be no empty";
      this._cdr.detectChanges()
      return;
    }

    if (_data["copyright"] === "") {
      this.errorMessage = "Field copyright must be no empty";
      this._cdr.detectChanges()
      return;
    }
    console.log("_data: ", _data);

    this._spinner.show();
    this._fhq.api().quest_proposal(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
    // _spinnerService

    console.log('todo send');
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this.successSended = true;
    this._cdr.detectChanges();
    this._spinner.hide();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }
}
