import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { MapActivityComponent } from './map-activity.component';

describe('MapActivityComponent', () => {
  let component: MapActivityComponent;
  let fixture: ComponentFixture<MapActivityComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ MapActivityComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(MapActivityComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
