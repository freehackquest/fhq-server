import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FhqService } from 'src/app/services/fhq.service';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'app-quests-by-subject',
  templateUrl: './quests-by-subject.component.html',
  styleUrls: ['./quests-by-subject.component.css']
})
export class QuestsBySubjectComponent implements OnInit {
  errorMessage: string = null;
  dataList: Array<any> = [];
  subject: string = "";
  subscription: any = null;
  count: number = 0;

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _route: ActivatedRoute,
    private _router: Router,
  ) { }

  ngOnInit() {
    this._route.params.subscribe( (params) => {
      if (!params['subject']) {
        this._router.navigate(['/quests', 'admin']);
        return;
      }
      this.subject = params['subject'];
      // TODO check possible subjects
      this.loadData();

      this.subscription = this._fhq.changedState
        .subscribe(() => this.loadData());
    });
  }

  ngOnDestroy() {
    if (this.subscription != null) {
      this.subscription.unsubscribe();
    }
  }

  loadData() {
    const _data = {
      subject: this.subject
    }
    this._spinner.show();
    this._fhq.api().quests(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinner.hide();

    this.dataList = []
    r.data.forEach((el: any) => {  
      this.dataList.push(el);
    });
    this.count = this.dataList.length;
    
    this.dataList.sort((a:any, b: any) => {
      if (a.status === "open" && b.status === "completed") {
        return -1;
      }
      if (a.status === "completed" && b.status === "open") {
        return 1;
      }
      return b.solved - a.solved;
    });

    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    this._spinner.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }

  openQuest(questid: number) {
    window.open("/?quest=" + questid, "_blank");
  }
}
