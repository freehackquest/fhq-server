import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { UserSkillsComponent } from './user-skills.component';

describe('UserSkillsComponent', () => {
  let component: UserSkillsComponent;
  let fixture: ComponentFixture<UserSkillsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ UserSkillsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UserSkillsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
