import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CtfWallComponent } from './ctf-wall.component';

describe('CtfWallComponent', () => {
  let component: CtfWallComponent;
  let fixture: ComponentFixture<CtfWallComponent>;

  beforeEach(async(() => {
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
