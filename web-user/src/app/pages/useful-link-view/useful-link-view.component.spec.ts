import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { UsefulLinkViewComponent } from './useful-link-view.component';

describe('UsefulLinkViewComponent', () => {
  let component: UsefulLinkViewComponent;
  let fixture: ComponentFixture<UsefulLinkViewComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ UsefulLinkViewComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UsefulLinkViewComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
