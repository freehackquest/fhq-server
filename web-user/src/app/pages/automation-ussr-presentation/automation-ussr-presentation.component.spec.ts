import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { AutomationUssrPresentationComponent } from './automation-ussr-presentation.component';

describe('AutomationUssrPresentationComponent', () => {
  let component: AutomationUssrPresentationComponent;
  let fixture: ComponentFixture<AutomationUssrPresentationComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ AutomationUssrPresentationComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AutomationUssrPresentationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
