import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { UserTokensComponent } from './user-tokens.component';

describe('UserTokensComponent', () => {
  let component: UserTokensComponent;
  let fixture: ComponentFixture<UserTokensComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ UserTokensComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UserTokensComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
