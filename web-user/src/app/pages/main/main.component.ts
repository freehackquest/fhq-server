import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';

@Component({
  selector: 'app-main',
  templateUrl: './main.component.html',
  styleUrls: ['./main.component.css']
})
export class MainComponent implements OnInit {

  darkTheme: boolean = false;
  cssId: string = "cyborgTheme";
  errorMessage: string = null;
  statisticsUsersOnline: number = 0;
  statisticsCountQuests: number = 0;
  statisticsAllAttempts: number = 0;
  statisticsAlreadySolved: number = 0;
  winners: Array<any> = [];
  cities: Array<any> = [];

  subscription: any;
  constructor(
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _spinnerService: SpinnerService,
    public _activeModal: NgbActiveModal,
  ) { }

  ngOnInit() {
    this.updatePage();
    this.subscription = this._fhq.changedState
      .subscribe(() => this.updatePage());
  }

  ngOnDestroy() {
    this.subscription.unsubscribe();
  }
  
  updatePage() {
    this._spinnerService.show();
    this.errorMessage = null;
    this.winners = [];
    this.cities = [];

    this._fhq.api().public_info({})
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));

    if (this._fhq.isAuthorized) {
      // this.userId = parseInt(this._fhq.userdata.id, 10);
      
    } else {
      // this.userId = 0;
      // this.userSkills = [];
    }
    this._spinnerService.hide();
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this.statisticsUsersOnline = r.connectedusers;
		this.statisticsCountQuests = r.quests.count;
		this.statisticsAllAttempts = r.quests.attempts;
    this.statisticsAlreadySolved = r.quests.solved;
    this.winners = []
    r.winners.forEach((w: any) => {
      this.winners.push(w);
    });
    
    r.cities.forEach((w: any) => {
      this.cities.push(w);
    });

    this._spinnerService.hide();
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }


  themeSwitcher() {
    if (this.darkTheme) {
      this.darkTheme = false;
      (document.getElementById(this.cssId) as any).disabled = true;
    } else {
      this.darkTheme = true;
      if (!document.getElementById(this.cssId)) {
          var head  = document.getElementsByTagName('head')[0];
          var link  = document.createElement('link');
          link.id   = this.cssId;
          link.rel  = 'stylesheet';
          link.type = 'text/css';
          link.href = 'assets/cyborgTheme/bootstrap.min.css';
          link.media = 'all';
          head.appendChild(link);
      } else {
          (document.getElementById(this.cssId) as any).disabled = false;///i fit's already there, enable it
      }
    }

    /*
    var dark_switcher = '';
	if (localStorage && localStorage.getItem('dark') != null) {
		dark_switcher = '<a href="javascript:void(0);" onclick="localStorage.removeItem(\'dark\'); window.location.reload();">Default theme</a>'
	} else {
		dark_switcher = '<a href="javascript:void(0);" onclick="localStorage.setItem(\'dark\', true); window.location.reload();">Cyborg theme</a>'
	}
    */
  }

}
