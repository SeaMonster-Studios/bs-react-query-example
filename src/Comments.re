open Belt;

[@decco]
type comment = {
  id: string,
  postId: string,
  body: string,
  userId: string,
  createdAt: string,
  user: option(Users.user),
};

[@decco]
type comments = array(comment);

module Queries = {
  module Query = {
    include ReactQuery.Query({
      type key = string;
      type data = Result.t(comments, Decco.decodeError);
      type vars = unit;
    });

    let getEndpoint = postId => {j|/api/comments/?_expand=user&postId=$postId&_sort=createdAt&_order=desc|j};

    let use = (postId: option(string)) => {
      use(~key=postId->Option.map(postId => postId->getEndpoint), (key, _) =>
        Js.Promise.(
          Fetch.fetch(key)
          |> then_(Fetch.Response.json)
          |> then_(json => json->comments_decode->resolve)
        )
      );
    };
  };

  module Create = {
    include ReactQuery.Mutation({
      type data = Result.t(comment, Decco.decodeError);
      type vars = comment;
    });

    let use = () => {
      use(
        ~options=
          Options.(
            make(
              ~onSuccess=
                (data, _) => {
                  data->Belt.Result.mapWithDefault(
                    (),
                    comment => {
                      let default =
                        [|comment|]->Belt.Result.Ok->Js.Nullable.return;
                      Query.Cache.setData(
                        comment.postId->Query.getEndpoint, comments =>
                        comments
                        ->Js.Nullable.toOption
                        ->Option.mapWithDefault(default, comments =>
                            comments->Belt.Result.mapWithDefault(
                              default, comments =>
                              [|comment|]
                              ->Array.concat(comments)
                              ->Belt.Result.Ok
                              ->Js.Nullable.return
                            )
                          )
                      );
                    },
                  );
                  Js.Nullable.null;
                },
              (),
            )
          ),
        comment =>
        Js.Promise.(
          Fetch.fetchWithInit(
            "/api/comments",
            Fetch.RequestInit.make(
              ~method_=Post,
              ~headers=
                Fetch.HeadersInit.make({
                  "Content-Type": "application/json",
                  "Accept": "application/json",
                }),
              ~body=
                Fetch.BodyInit.make(
                  comment->comment_encode->Js.Json.stringify,
                ),
              (),
            ),
          )
          |> then_(Fetch.Response.json)
          |> then_(json => json->comment_decode->resolve)
        )
      );
    };
  };
};

[@react.component]
let make = (~postId) => {
  let commentsQ = postId->Some->Queries.Query.use;

  switch (commentsQ.status) {
  | Loading => <Loader />
  | Error(err) =>
    err->Js.log;
    <ErrorMessage />;
  | Success(Error(err)) =>
    err->Js.log;
    <ErrorMessage />;
  | Success(Ok(comments)) =>
    comments
    ->Array.map(comment =>
        comment.user
        ->Option.mapWithDefault(
            <React.Fragment key={comment.id}> React.null </React.Fragment>,
            author =>
            <Card author key={comment.id}> comment.body->React.string </Card>
          )
      )
    ->React.array
  };
};