import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { AutomationUssrRatingComponent } from './automation-ussr-rating.component';

describe('AutomationUssrRatingComponent', () => {
  let component: AutomationUssrRatingComponent;
  let fixture: ComponentFixture<AutomationUssrRatingComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ AutomationUssrRatingComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AutomationUssrRatingComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
