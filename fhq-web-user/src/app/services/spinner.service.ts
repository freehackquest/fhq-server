import { Overlay, OverlayRef } from '@angular/cdk/overlay';
import { ComponentPortal } from '@angular/cdk/portal';
import { Injectable } from '@angular/core';
import { SpinnerComponent } from '../spinner/spinner.component';

@Injectable({
  providedIn: 'root'
})
export class SpinnerService {

  private overlayRef: OverlayRef = null;
  private showed: boolean = false;

  constructor(private overlay: Overlay) {}

  public show(message = '') {
    if (this.showed) {
      console.warn("Already showed");
      return;
    }
    // Returns an OverlayRef (which is a PortalHost)
    this.showed = true;

    if (!this.overlayRef) {
      this.overlayRef = this.overlay.create();
    }

    // Create ComponentPortal that can be attached to a PortalHost
    const spinnerOverlayPortal = new ComponentPortal(SpinnerComponent);
    const component = this.overlayRef.attach(spinnerOverlayPortal); // Attach ComponentPortal to PortalHost
  }

  public isShowed() {
    return this.showed;
  }

  public hide() {
    if (!!this.overlayRef) {
      this.overlayRef.detach();
      this.showed = false;
    }
  }
}
