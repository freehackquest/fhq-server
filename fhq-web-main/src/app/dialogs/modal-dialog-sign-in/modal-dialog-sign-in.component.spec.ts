import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ModalDialogSignInComponent } from './modal-dialog-sign-in.component';

describe('ModalDialogSignInComponent', () => {
  let component: ModalDialogSignInComponent;
  let fixture: ComponentFixture<ModalDialogSignInComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalDialogSignInComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalDialogSignInComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
