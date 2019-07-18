import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-user-skills',
  templateUrl: './user-skills.component.html',
  styleUrls: ['./user-skills.component.css']
})
export class UserSkillsComponent implements OnInit {
  userSkills: Array<any> = [];
  userId: number = 0;
  resultOfUserSkills: string = null;
  subscription: any;

  constructor(
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _spinnerService: SpinnerService,
  ) {

  }

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
      this.userId = parseInt(this._fhq.userdata.id, 10);
      this.loadUserSkills();
    }
    else {
      this.userId = 0;
      this.userSkills = [];
    }
  }

  loadUserSkills() {
    this._fhq.api().user_skills({
      "userid": this.userId,
    }).done((r: any) => this.successUserSkills(r))
      .fail((err: any) => this.errorUserSkills(err));
  }

  successUserSkills(r: any) {
    this.userSkills = [];
    for (let user_skill in r.skills_user) {
      let skill = {};
      skill['name'] = user_skill;
      skill['max'] = r.skills_max[user_skill];
      skill['val'] = r.skills_user[user_skill];
      skill['procent'] = Math.floor(100 * (skill['val'] / skill['max']));
      this.userSkills.push(skill);
    }
    this._cdr.detectChanges();
    this._spinnerService.hide();
  }

  errorUserSkills(err: any) {
    console.error("errorUserSkills: ", err);
  }
}
