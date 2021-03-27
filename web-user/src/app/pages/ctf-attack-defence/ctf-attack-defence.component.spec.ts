import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CtfAttackDefenceComponent } from './ctf-attack-defence.component';

describe('CtfAttackDefenceComponent', () => {
  let component: CtfAttackDefenceComponent;
  let fixture: ComponentFixture<CtfAttackDefenceComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ CtfAttackDefenceComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CtfAttackDefenceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
