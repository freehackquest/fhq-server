import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { FhqJuryAdComponent } from './fhq-jury-ad.component';

describe('FhqJuryAdComponent', () => {
  let component: FhqJuryAdComponent;
  let fixture: ComponentFixture<FhqJuryAdComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ FhqJuryAdComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(FhqJuryAdComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
