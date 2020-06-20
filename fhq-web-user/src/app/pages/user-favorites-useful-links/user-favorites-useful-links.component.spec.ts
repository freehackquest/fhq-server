import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { UserFavoritesUsefulLinksComponent } from './user-favorites-useful-links.component';

describe('UserFavoritesUsefulLinksComponent', () => {
  let component: UserFavoritesUsefulLinksComponent;
  let fixture: ComponentFixture<UserFavoritesUsefulLinksComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ UserFavoritesUsefulLinksComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UserFavoritesUsefulLinksComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
