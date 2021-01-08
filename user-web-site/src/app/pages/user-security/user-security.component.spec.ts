import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { UserSecurityComponent } from './user-security.component';

describe('UserSecurityComponent', () => {
  let component: UserSecurityComponent;
  let fixture: ComponentFixture<UserSecurityComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ UserSecurityComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UserSecurityComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
