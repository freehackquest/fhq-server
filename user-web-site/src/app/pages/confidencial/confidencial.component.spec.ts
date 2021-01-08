import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ConfidencialComponent } from './confidencial.component';

describe('ConfidencialComponent', () => {
  let component: ConfidencialComponent;
  let fixture: ComponentFixture<ConfidencialComponent>;

  beforeEach(async(() => {
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
