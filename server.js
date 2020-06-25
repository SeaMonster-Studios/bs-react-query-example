const faker = require("faker")
const _ = require("lodash")
const jsonServer = require('json-server')
const server = jsonServer.create()
const middlewares = jsonServer.defaults()

const makeData = () => {

  const authedUser = {
    id: faker.random.uuid(),
    name: "Carlton Lassiter",
    avatar: faker.image.avatar()
  };

  const users = _.times(20, () => ({
    id: faker.random.uuid(),
    name: `${faker.name.firstName()} ${faker.name.lastName()},`,
    avatar: faker.image.avatar()
  }));

  let postId = faker.random.uuid();


  const data = {
    users: [authedUser, ...users],
    posts: [
      {
        id: postId, title: faker.name.title(), body: faker.lorem.paragraphs(), userId: authedUser.id,
        createdAt: faker.date.past().toISOString()
      }
    ],
    comments: _.times(5, () => ({
      id: faker.random.uuid(),
      postId,
      createdAt: faker.date.past().toISOString(),
      body: faker.lorem.sentences(),
      userId: users[Math.floor(Math.random() * users.length)].id
    }))
  }
  return data
}

const router = jsonServer.router(makeData())

server.use(middlewares)

// To handle POST, PUT and PATCH you need to use a body-parser
// You can use the one used by JSON Server
server.use(jsonServer.bodyParser)
server.use((req, res, next) => {
  if (req.method === 'POST') {
    req.body.createdAt = new Date().toISOString()
  }
  // Continue to JSON Server router
  next()
})

// Use default router
server.use(router)
server.listen(3000, () => {
  console.log('JSON Server is running')
})