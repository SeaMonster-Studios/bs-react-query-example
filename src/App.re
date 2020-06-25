open MaterialUi;

module Style = {
  open Css;
  let main =
    [
      padding(60->px),
      display(`flex),
      alignItems(`center),
      minHeight(100.->vh),
    ]
    ->style;
};

[@react.component]
let make = () => {
  let queryConfig =
    React.useMemo0(() =>
      ReactQuery.ConfigProvider.(config(~onError=error => error->Js.log, ()))
    );

  <ReactQuery.ConfigProvider config=queryConfig>
    <Typography />
    <CssBaseline />
    <AppBar>
      <Container maxWidth=Container.MaxWidth.sm>
        <h1> "bs-react-query example"->React.string </h1>
      </Container>
    </AppBar>
    <main className=Style.main>
      <Container maxWidth=Container.MaxWidth.sm> <Posts /> </Container>
    </main>
  </ReactQuery.ConfigProvider>;
};