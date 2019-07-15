import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalDialogQuestFeedbackComponent } from './modal-dialog-quest-feedback.component';

describe('ModalDialogQuestFeedbackComponent', () => {
  let component: ModalDialogQuestFeedbackComponent;
  let fixture: ComponentFixture<ModalDialogQuestFeedbackComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalDialogQuestFeedbackComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalDialogQuestFeedbackComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
