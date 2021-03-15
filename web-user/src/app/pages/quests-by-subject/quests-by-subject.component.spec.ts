import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { QuestsBySubjectComponent } from './quests-by-subject.component';

describe('QuestsBySubjectComponent', () => {
  let component: QuestsBySubjectComponent;
  let fixture: ComponentFixture<QuestsBySubjectComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ QuestsBySubjectComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(QuestsBySubjectComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
