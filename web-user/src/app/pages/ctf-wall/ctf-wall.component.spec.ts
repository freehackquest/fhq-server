import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { CtfWallComponent } from './ctf-wall.component';

describe('CtfWallComponent', () => {
  let component: CtfWallComponent;
  let fixture: ComponentFixture<CtfWallComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ CtfWallComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CtfWallComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
