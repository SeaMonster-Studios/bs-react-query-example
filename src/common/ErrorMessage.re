module Style = {
  open Css;

  let root =
    [
      padding(15->px),
      backgroundColor(rgb(220, 0, 78)),
      color(`hex("fff")),
    ]
    ->style;
};

[@react.component]
let make = () =>
  <p className=Style.root> "Sorry! A problem occurred."->React.string </p>;