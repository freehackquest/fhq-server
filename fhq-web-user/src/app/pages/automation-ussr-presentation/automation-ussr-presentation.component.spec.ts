import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AutomationUssrPresentationComponent } from './automation-ussr-presentation.component';

describe('AutomationUssrPresentationComponent', () => {
  let component: AutomationUssrPresentationComponent;
  let fixture: ComponentFixture<AutomationUssrPresentationComponent>;

  beforeEach(async(() => {
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
