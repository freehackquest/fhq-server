import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { ModalSignInDialog } from './modal-signin-dialog.component';

describe('ModalSignInDialog', () => {
  let component: ModalSignInDialog;
  let fixture: ComponentFixture<ModalSignInDialog>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ ModalSignInDialog ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ModalSignInDialog);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
