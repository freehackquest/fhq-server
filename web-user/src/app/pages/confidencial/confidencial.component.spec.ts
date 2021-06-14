import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { ConfidencialComponent } from './confidencial.component';

describe('ConfidencialComponent', () => {
  let component: ConfidencialComponent;
  let fixture: ComponentFixture<ConfidencialComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ ConfidencialComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ConfidencialComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
