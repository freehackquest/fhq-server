import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-welcome',
  templateUrl: './welcome.component.html',
  styleUrls: ['./welcome.component.css']
})
export class WelcomeComponent implements OnInit {
  subscription: any;
  constructor(
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _spinnerService: SpinnerService,
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
    if (this._fhq.isAuthorized) {
      // this.userId = parseInt(this._fhq.userdata.id, 10);
      
    } else {
      // this.userId = 0;
      // this.userSkills = [];
    }
    this._spinnerService.hide();
  }

}
