import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AutomationUssrComponent } from './automation-ussr.component';

describe('AutomationUssrComponent', () => {
  let component: AutomationUssrComponent;
  let fixture: ComponentFixture<AutomationUssrComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AutomationUssrComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AutomationUssrComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
