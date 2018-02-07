open BsReactNative;
open PhotoGrid;

type state = {
  data: list(photo),
  page: int
};
type action =
  | UpdateData(Js.Json.t);
type fetchCallback = Js.Json.t => unit;

module Decode = {
  let urls = json =>
    Json.Decode.{
      full: json |> field("full", string),
      regular: json |> field("regular", string),
      small: json |> field("small", string),
      thumb: json |> field("thumb", string)
    };

  let photo = json => {
    Json.Decode.{
      id: json |> field("id", string),
      description: json |> optional(field("description", string)),
      likes: json |> field("likes", int),
      urls: json |> field("urls", urls)
    };
  };
};

let style = Style.({
  "app": style([
    flex(1.),
    justifyContent(Center),
    alignItems(Center),
    marginTop(Pt(20.))
  ])
});
let dataToList = (data) : list(photo) => {
  data
  |> Js.Array.map(o => Decode.photo(o))
  |> Array.to_list;
};

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  let formatData = (json) =>
    switch (Js.Json.decodeArray(json)) {
    | Some(data) => dataToList(data)
    | None => []
    };
  let fetchData = (~page: int, ~callback) =>
    Js.Promise.(
      ([@bs] Unsplash.listPhotos(page, 16, "trending"))
      |> then_(Fetch.Response.json)
      |> then_(json => resolve(callback(json)))
      |> catch((err) => resolve(Js.log(err)))
    );
  let fetchPhotos = (_, self) => {
    fetchData(~page=self.ReasonReact.state.page, ~callback=(json => {
      self.ReasonReact.reduce((_) => UpdateData(json))();
    }))
    |> ignore;
  };

  {
    ...component,
    initialState: () => {
      data: [],
      page: 1
    },
    reducer: (action, state) => {
      switch action {
      | UpdateData(json) =>
        let { page, data } = state;
        ReasonReact.Update({
          ...state,
          page: page + 1,
          data: List.append(data, formatData(json))
        })
      | _ => ReasonReact.NoUpdate;
      };
    },
    didMount: (self) => {
      fetchPhotos()(self);
      ReasonReact.NoUpdate;
    },
    render: ({state: {page, data}, handle}) => {
      <View style=style##app>
        <PhotoGrid
          photos=data
          fetchPhotos=handle(fetchPhotos)
        />
      </View>;
    }
  }
};

let jsComponent =
  ReasonReact.wrapReasonForJs(
    ~component,
    (_jsProps) => make([||])
  );
