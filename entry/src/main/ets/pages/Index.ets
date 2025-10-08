import nativeRender from 'libentry.so';
import { ArcButtonComponent } from '../components/ArcButtonComponent';
import { ArcButtonPosition } from '@kit.ArkUI';

@Entry
@Component
struct Index {
  private xComponentContext: ESObject | undefined = undefined;
  @State metaballCount: number = 0;
  @State isRendering: boolean = false;

  build() {
    Column() {

      ArcButtonComponent({
        label: 'Clear',
        arcPosition: ArcButtonPosition.TOP_EDGE,
        fontSize: 8,
        shadowEnabled: true,
        onButtonClick: () => {
          if (this.xComponentContext) {
            nativeRender.clearMetaballs(this.xComponentContext);
            this.metaballCount = 0;
          }
        }
      })
        .height('5%')
        .zIndex(1)
        .margin({ top: "0%" })


      XComponent({
        id: 'xcomponentId',
        type: XComponentType.SURFACE,
        libraryname: 'entry'
      })
        .onLoad((xComponentContext) => {
          this.xComponentContext = xComponentContext;
          this.isRendering = true;
        })
        .width('100%')
        .height('63%')
        .borderRadius(16)
        .clip(true)
        .margin({ top: "13%" })
        .shadow({
          radius: 20,
          color: 'rgba(0,0,0,0.3)',
          offsetX: 0,
          offsetY: 10
        })


      ArcButtonComponent({
        label: 'Add Random',
        arcPosition: ArcButtonPosition.BOTTOM_EDGE,
        fontSize: 8,
        shadowEnabled: true,
        onButtonClick: () => {

          if (this.xComponentContext) {
            const randomX = Math.random() * 540 + 30;
            const randomY = Math.random() * 540 + 30;
            nativeRender.addMetaball(this.xComponentContext, randomX, randomY);
            this.metaballCount++;
          }
        }
      })
        .height('7%')
        .margin({ bottom: "0%" })

    }
    .width('100%')
    .height('100%')
  }
}