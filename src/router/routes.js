const routes = [
  {
    path: '/',
<<<<<<< HEAD
    component: () => import('pages/IndexPage.vue'),
    children: [{ path: '', component: () => import('pages/IndexPage.vue') }],
=======
    component: () => import('layouts/MainLayout.vue'),
    children: [
      { path: '', component: () => import('pages/IndexPage.vue') }
    ]
>>>>>>> 142d6baa33a5bfc42e364dc235d701d137f013d1
  },

  // Always leave this as last one,
  // but you can also remove it
  {
    path: '/:catchAll(.*)*',
<<<<<<< HEAD
    component: () => import('pages/ErrorNotFound.vue'),
  },
=======
    component: () => import('pages/ErrorNotFound.vue')
  }
>>>>>>> 142d6baa33a5bfc42e364dc235d701d137f013d1
]

export default routes
