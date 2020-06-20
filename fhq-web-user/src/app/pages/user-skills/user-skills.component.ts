import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { MatTableDataSource } from '@angular/material';

export interface UserSkillsElement {
  skillName: string;
  userPoints: number;
  maxPoints: number;
  inProcents: number;
}

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
  dataSource = new MatTableDataSource<UserSkillsElement>();

  userSkillsData: UserSkillsElement[] = [];
  displayedColumns: string[] = ['name', 'userPoints'];

  constructor(
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _spinnerService: SpinnerService,
    private _modalService: NgbModal,
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
    } else {
      this.userId = 0;
      this.userSkills = [];
    }
    this._spinnerService.hide();
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

      this.userSkillsData.push({
        skillName: user_skill,
        maxPoints: r.skills_max[user_skill],
        userPoints: r.skills_user[user_skill],
        inProcents: Math.floor(100 * (skill['val'] / skill['max']))
      })
    }
    console.log(this.userSkillsData)
    this.dataSource = new MatTableDataSource<UserSkillsElement>(this.userSkillsData);
    
    this._cdr.detectChanges();
    this._spinnerService.hide();
  }

  errorUserSkills(err: any) {
    console.error("errorUserSkills: ", err);
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }
}
