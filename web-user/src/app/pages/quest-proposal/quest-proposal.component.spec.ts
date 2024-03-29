import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { QuestProposalComponent } from './quest-proposal.component';

describe('QuestProposalComponent', () => {
  let component: QuestProposalComponent;
  let fixture: ComponentFixture<QuestProposalComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ QuestProposalComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(QuestProposalComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
