open BsReactNative;

let styles = Style.({
  "modal": style([
    backgroundColor("#000")
  ]),
  "activePhoto": style([
    flex(1.),
    justifyContent(Center)
  ])
});

let component = ReasonReact.statelessComponent("PhotoModal");
let make = (~photoUrl, ~isVisible, ~onDismiss, _children) => {
  {
    ...component,
    render: _self =>
      <Modal animationType=`slide visible=isVisible>
        <TouchableWithoutFeedback onPress=onDismiss style=styles##modal>
          <Image
            style=styles##activePhoto
            resizeMode=`contain
            source=URI(Image.(imageURISource(~uri=photoUrl, ())))
          />
        </TouchableWithoutFeedback>
      </Modal>
  }
};
