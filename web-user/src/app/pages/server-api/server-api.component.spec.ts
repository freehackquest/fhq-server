import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { ServerApiComponent } from './server-api.component';

describe('ServerApiComponent', () => {
  let component: ServerApiComponent;
  let fixture: ComponentFixture<ServerApiComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ ServerApiComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ServerApiComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
