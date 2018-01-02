open BsReactNative;

type urls = {
  full: string,
  regular: string,
  small: string,
  thumb: string
};
type photo = {
  id: string,
  description: option(string),
  likes: int,
  urls: urls
};
type props = {
  photos: list(photo),
  fetchPhotos: unit => unit
};
type state = {
  canFetch: bool,
  selectedPhoto: option(photo)
};
type action =
  | SetFetch(bool)
  | SelectPhoto(option(photo))
  | NoUpdate;

let windowHeight = float_of_int(Dimensions.get(`window)##height);
let windowWidth = float_of_int(Dimensions.get(`window)##width);
let thumbWidth = windowWidth /. 2.;
let styles = Style.({
  "container": style([
    flex(1.),
    flexDirection(Row),
    flexWrap(Wrap),
    justifyContent(FlexStart),
    alignItems(Center),
    margin(Pt(0.))
  ]),
  "photo": style([
    width(Pt(thumbWidth)),
    height(Pt(thumbWidth)),
    flexShrink(1.),
    alignSelf(Center)
  ])
});

let component = ReasonReact.reducerComponentWithRetainedProps("PhotoGrid");
let make = (~photos, ~fetchPhotos, _children) => {
  let renderPhoto = self => (photo: photo) =>
    <Lightbox
      onOpen=self.ReasonReact.reduce((_) => SelectPhoto(Some(photo)))
      onClose=self.ReasonReact.reduce((_) => SelectPhoto(None))
      activeProps=({"width": windowWidth, "flex": 1, "resizeMode": "contain"})
    >
      <Image
        style=styles##photo
        resizeMode=`cover
        source=URI(Image.(imageURISource(~uri=photo.urls.regular, ())))
      />
    </Lightbox>;
  let renderPhotos = (photos, self) =>
    switch (List.length(photos)) {
    | 0 =>
      [|<Text value="No photos..." />|]
    | _ => Array.of_list(List.map(renderPhoto(self), photos))
    };
  let onScroll = canFetch => (event: RNEvent.NativeScrollEvent.t) => {
    let contentHeight = RNEvent.NativeScrollEvent.contentSize(event).height;
    let contentOffsetY = RNEvent.NativeScrollEvent.contentOffset(event).y;
    let isEndReached = contentOffsetY +. windowHeight >= contentHeight +. 10.;

    if (isEndReached && canFetch) {
      fetchPhotos();
      SetFetch(false);
    } else {
      NoUpdate;
    };
  };

  {
    ...component,
    initialState: () => {
      canFetch: true,
      selectedPhoto: None
    },
    retainedProps: {photos, fetchPhotos},
    willReceiveProps: (self) => {
      switch (List.length(photos) > List.length(self.retainedProps.photos)) {
      | true => {...self.state, canFetch: true}
      | false => self.state
      };
    },
    reducer: (action, state) => {
      switch action {
      | SetFetch(canFetch) => ReasonReact.Update({...state, canFetch})
      | SelectPhoto(photo) => ReasonReact.Update({...state, selectedPhoto: photo})
      | _ => ReasonReact.NoUpdate;
      };
    },
    render: (self) =>
      <ScrollView onScroll=self.reduce(onScroll(self.state.canFetch)) scrollEventThrottle=16>
        <View style=styles##container>
          ...(renderPhotos(photos, self))
        </View>
      </ScrollView>
  };
};
