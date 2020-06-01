import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { MatDrawer } from '@angular/material/sidenav';
import { MatTableDataSource } from '@angular/material';


export interface UserSkillsElement {
  skillName: string;
  userPoints: number;
  maxPoints: number;
  inProcents: number;
}
/*
const ELEMENT_DATA: UserSkillsElement[] = [
  {position: 1, name: 'Hydrogen', weight: 1.0079, symbol: 'H'},
  {position: 2, name: 'Helium', weight: 4.0026, symbol: 'He'},
  {position: 3, name: 'Lithium', weight: 6.941, symbol: 'Li'},
  {position: 4, name: 'Beryllium', weight: 9.0122, symbol: 'Be'},
  {position: 5, name: 'Boron', weight: 10.811, symbol: 'B'},
  {position: 6, name: 'Carbon', weight: 12.0107, symbol: 'C'},
  {position: 7, name: 'Nitrogen', weight: 14.0067, symbol: 'N'},
  {position: 8, name: 'Oxygen', weight: 15.9994, symbol: 'O'},
  {position: 9, name: 'Fluorine', weight: 18.9984, symbol: 'F'},
  {position: 10, name: 'Neon', weight: 20.1797, symbol: 'Ne'},
];
*/

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

  @ViewChild('drawer', { static: true }) drawer: MatDrawer;

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
    this.drawer.open();
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
}
